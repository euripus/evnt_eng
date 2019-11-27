// movabe and non-copyable
temlate<typename GeneratorType>
class GlObjectGen
{
    GeneratorType m_gen;
    GLuint  obj_id{0};
    
public:
    GlObjectGen(bool create)
    {
        if(create)
            obj_id = m_gen.Create();
    }
    
    ~GlObjectGen()
    {
        if(obj_id != 0)
        {
            m_gen.Release(obj_id);
            obj_id = 0;
        }
    }
    
    GlObjectGen(const GlObjectGen &) = delete;
    GlObjectGen & operator=(const GlObjectGen &) = delete;
    
    GlObjectGen& operator=(GlObjectGen && other)
    {
        if(obj_id != 0)
        {
            m_gen.Release(obj_id);
            obj_id = 0;
        }
        
        std::swap(obj_id, other.obj_id);
        std::swap(m_gen, other.m_gen);
        return *this;
    }
    
    GlObjectGen(GlObjectGen && other)
    {
        obj_id = other.obj_id;
        m_gen = other.m_gen;
    }
    
    operator GLuint() const {return obj_id;}
    
    static GlObjectGen Null()
    {
        return GlObjectGen{false};
    }
};

class TexGen
{
public:
    GLuint Create() const
    {
        GLuint new_tex{0};
        glGenTextures(1, &new_tex);
        
        return new_tex;
    }
    
    void Release(GLuint tex)
    {
        glDeleteTextures(1, &tex);
    }
};
using TexObject = GlObjectGen<TexGen>;
        
